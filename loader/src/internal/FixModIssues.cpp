#include "FixModIssues.hpp"
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/Popup.hpp>
#include <server/DownloadManager.hpp>
#include <ui/mods/sources/ModSource.hpp>
#include <vector>
#include <deque>

// TODO: UNFINISHED!!!
// If you want to bring this back, you are free to do so - 
// I just didn't feel like the engineering effort is worth it.
// The point of this is to be a mod load issue auto-resolver

using namespace geode::prelude;

class AutoFixStatus final {
protected:
    struct Question final {
        std::string title;
        std::string content;
        std::string optionA;
        std::string optionB;
        MiniFunction<void(bool)> after;
    };

    EventListener<server::ModDownloadFilter> m_download;
    std::vector<std::string> m_unsolved;
    std::deque<Question> m_questionQueue;

    static std::filesystem::path getPath(LoadProblem const& problem) {
        return std::visit(makeVisitor {
            [](std::filesystem::path const& path) {
                return path;
            },
            [](ModMetadata const& meta) {
                return meta.getPath();
            },
            [](Mod* mod) {
                return mod->getPackagePath();
            },
        }, problem.cause);
    }
    static std::string getName(LoadProblem const& problem) {
        return std::visit(makeVisitor {
            [](std::filesystem::path const& path) {
                return path.string();
            },
            [](ModMetadata const& meta) {
                return meta.getID();
            },
            [](Mod* mod) {
                return mod->getID();
            },
        }, problem.cause);
    }

    void nextQuestion() {
        auto& question = m_questionQueue.front();
        createQuickPopup(
            question.title.c_str(),
            question.content,
            question.optionA.c_str(), question.optionB.c_str(),
            [this, &question](auto*, bool btn2) {
                question.after(btn2);
                m_questionQueue.pop_front();
                if (!m_questionQueue.empty()) {
                    this->nextQuestion();
                }
            }
        );
    }
    void ask(Question&& question) {
        m_questionQueue.push_back(question);
        // If this was the first question in the queue, start asking
        if (m_questionQueue.size() == 1) {
            this->nextQuestion();
        }
    }

public:
    void start() {
        for (auto problem : Loader::get()->getProblems()) {
            switch (problem.type) {
                // Errors where the correct solution is to just delete the invalid .geode package
                case LoadProblem::Type::InvalidFile:
                // todo: maybe duplicate should prompt which one to delete?
                // or maybe the user can just figure that one out since that only happens 
                // on manual install (as server installs delete the old version using the 
                // real path and not the old index filename trickery)
                case LoadProblem::Type::Duplicate:
                case LoadProblem::Type::SetupFailed:
                case LoadProblem::Type::LoadFailed:
                case LoadProblem::Type::EnableFailed:
                case LoadProblem::Type::UnsupportedGeodeVersion:
                case LoadProblem::Type::NeedsNewerGeodeVersion:
                case LoadProblem::Type::UnsupportedVersion:
                {
                    auto path = getPath(problem);
                    std::error_code ec;
                    std::filesystem::remove(path, ec);
                    if (ec) {
                        m_unsolved.push_back(fmt::format("Failed to delete '{}'", path));
                    }
                }
                break;

                // Missing / bad dependencies
                case LoadProblem::Type::MissingDependency:
                case LoadProblem::Type::OutdatedDependency:
                {
                    // Parse the damn "{id} {version}" string to get the id
                    // God I wish this was Rust so the enum variant could just have the ID right there
                    auto id = problem.message.substr(0, problem.message.find(' '));

                    // If this mod is already installed, see if it can be updated
                    if (auto mod = Loader::get()->getInstalledMod(id)) {
                        // todo: after update check, if there are no updates, mark this as unsolved, otherwise start update
                        (void)ModSource(mod).checkUpdates();
                    }
                    // Otherwise try to install the mod
                    // todo: Check if the mod can be downloaded, and if not mark this as unsolved
                    else {
                        server::ModDownloadManager::get()->startDownload(id, std::nullopt);
                    }
                }
                break;

                // Enable the dependency duh
                case LoadProblem::Type::DisabledDependency:
                {
                    auto mod = std::get<Mod*>(problem.cause);
                    if (!mod->enable()) {
                        m_unsolved.push_back(fmt::format("Failed to enable '{}'", mod->getID()));
                    }
                }
                break;

                // Incompatabilities; the user should choose which to disable
                case LoadProblem::Type::PresentIncompatibility:
                case LoadProblem::Type::OutdatedIncompatibility:
                case LoadProblem::Type::OutdatedConflict:
                case LoadProblem::Type::Conflict:
                {
                    auto modA = std::get<Mod*>(problem.cause)->getID();
                    auto modB = problem.message;
                    this->ask(Question {
                        .title = "Select Mod",
                        .content = fmt::format(
                            "The mods <cy>'{}'</c> and <cp>{}</c> are <cr>incompatible</c>.\n"
                            "<cj>Please select which one to disable</c>.",
                            modA, modB
                        ),
                        .optionA = modA,
                        .optionB = modB,
                        .after = [modA, modB](bool b) {

                        }
                    });
                }
                break;

                // Errors we can't fix, or ones where you should probably just restart the game / PC
                default:
                case LoadProblem::Type::UnzipFailed:
                case LoadProblem::Type::Unknown:
                {
                    auto name = getName(problem);
                    m_unsolved.push_back(fmt::format(
                        "<cr>Unknown/unsolvable error</c> with <cg>'{}'</c>: {}\n"
                        "<cy>Maybe try restarting your computer?</c>",
                        name, problem.message
                    ));
                }
                break;
            }
        }
    }
};

static std::optional<AutoFixStatus> STATUS = {};

void internal::tryAutoFixModIssues() {
    if (!STATUS) {
        STATUS.emplace(AutoFixStatus());
        STATUS->start();
    }
}
bool internal::hasTriedToFixIssues() {
    return STATUS.has_value();
}
