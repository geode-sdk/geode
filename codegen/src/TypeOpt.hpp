#include "Shared.hpp"
#include <set>
#include <algorithm>

namespace {
enum class FuncType : int {
	Structor,
	Static,
	Member
};
struct Func {
	std::string return_type;
	bool is_const;
	std::string class_name;
	std::vector<std::string> parameter_types;
	FuncType type;

	std::string toStr() const {
		return fmt::format("{}{}{}{}{}",
			return_type,
			class_name,
			fmt::join(parameter_types, ","),
			is_const ? " const " : "",
			static_cast<int>(type)
		);
	}

	bool operator<(Func const& f) const {
		return toStr() < f.toStr();
	}

	bool operator==(Func const& f) const {
		return (
			return_type == f.return_type &&
			is_const == f.is_const &&
			class_name == f.class_name &&
			parameter_types == f.parameter_types &&
			type == f.type
		);
	}
};

struct Ids {
	int ret = -1;
	int func = -1;
	int pure = -1;
	int meta = -1;
	int member = -1;
};

class TypeBank {
 	std::vector<Func> m_stuff;
 public:
	static std::string getReturn(FunctionBegin const& fn, std::string const& parent) {
		if (fn.type != FunctionType::Normal)
			return "void";

		if (fn.ret.name == "auto") {
			std::vector<std::string> declvals;

			for (auto& [t, n] : fn.args) {
				declvals.push_back(fmt::format("std::declval<{}>()", t.name));
			}

			return fmt::format(
				fn.is_static ? "decltype({}::{}({}))" : "decltype(std::declval<{}>().{}({}))",
				parent,
				fn.name,
				fmt::join(declvals, ", ")
			);
		}

		return fn.ret.name;
	}

	static Func makeFunc(FunctionBegin const& fn, std::string const& parent) {
		Func f;
		f.return_type = TypeBank::getReturn(fn, parent);
		f.is_const = fn.is_const;
		f.class_name = parent;

		for (auto& arg : fn.args) {
			f.parameter_types.push_back(arg.first.name);
		}

		if (fn.is_static)
			f.type = FuncType::Static;
		else if (fn.type != FunctionType::Normal)
			f.type = FuncType::Structor;
		else
			f.type = FuncType::Member;

		return f;
	}

	void loadFrom(Root& root) {
		for (auto& c : root.classes) {
			for (auto& f : c.fields) {
				if (codegen::getStatus(f) == BindStatus::Unbindable)
					continue;

				m_stuff.push_back(TypeBank::makeFunc(*f.get_fn(), c.name));
			}
		}

		std::sort(m_stuff.begin(), m_stuff.end());
		m_stuff.erase(std::unique(m_stuff.begin(), m_stuff.end()), m_stuff.end());
	}

	std::vector<Func> const& typeList() { return m_stuff; }

	Ids getIDs(FunctionBegin const& fn, std::string const& parent) {
		Ids out;
		Func in_f = TypeBank::makeFunc(fn, parent);

		int i = 0;
		for (auto f : m_stuff) {
			if (out.ret == -1 && f.return_type == in_f.return_type) {
				out.ret = i;
			}

			if (out.func == -1) {
				if (in_f.type == FuncType::Member) {
					if (f == in_f) out.func = i;
				} else if (in_f.type == FuncType::Structor) {
					if (
						f.return_type == in_f.return_type &&
						f.class_name == in_f.class_name &&
						f.is_const == in_f.is_const &&
						f.parameter_types == in_f.parameter_types
					) out.func = i;
				} else {
					if (
						f.return_type == in_f.return_type &&
						f.parameter_types == in_f.parameter_types &&
						f.type == in_f.type
					) out.func = i;
				}
			}

			if (out.meta == -1 || out.member == -1) {
				Func assume_member = f;
				assume_member.type = FuncType::Member;

				if (in_f == assume_member) {
					out.meta = i;
				} else if (out.func != -1) {
					out.meta = out.func;
				}
			}

			if (out.pure == -1 && f.return_type == in_f.return_type && f.parameter_types == in_f.parameter_types) {
				out.pure = i;
			}

			if (out.ret != -1 && out.func != -1 && out.pure != -1)
				break;
			++i;
		}

		out.member = out.func;

		return out;
	}

	int getPure(FunctionBegin const& fn, std::string const& parent) {
		Func in_f = TypeBank::makeFunc(fn, parent);

		int i = 0;
		for (auto f : m_stuff) {
			if (f.return_type == in_f.return_type && f.parameter_types == in_f.parameter_types) {
				return i;
			}
			++i;
		}

		return -1;
	}
};
}