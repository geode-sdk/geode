use std::fmt;
use std::num::ParseIntError;

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum VersionCompare {
    LessEq,
    Exact,
    MoreEq,
    Less,
    More,
    Any,
}

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum VersionCompareResult {
    TooOld,
    Match,
    TooNew,
    MajorMismatch,
    GenericMismatch,
}

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord, Clone)]
pub enum VersionTag {
    Alpha,
    Beta,
    Prerelease,
}

#[derive(Ord, Eq, PartialEq, PartialOrd, Debug, Clone)]
pub struct VersionTagType(pub VersionTag);

#[derive(Debug, Clone, Eq, Ord)]
pub struct VersionTagWithNumber {
    pub value: VersionTagType,
    pub number: Option<usize>,
}

impl VersionTagWithNumber {
    pub fn parse(tag_str: &str) -> Result<VersionTagWithNumber, ParseIntError> {
        let mut parts = tag_str.split('-');
        let value = parts
            .next()
            .ok_or_else(|| "Invalid version tag")
            .expect("fuck.");
        let number = parts.next().map(|n| n.parse()).transpose()?;
        let value = match value {
            "alpha" => VersionTagType(VersionTag::Alpha),
            "beta" => VersionTagType(VersionTag::Beta),
            "prerelease" => VersionTagType(VersionTag::Prerelease),
            _ => todo!(),
        };
        Ok(VersionTagWithNumber { value, number })
    }

    pub fn to_suffix_string(&self) -> String {
        match &self.value {
            VersionTagType(VersionTag::Alpha) => "alpha",
            VersionTagType(VersionTag::Beta) => "beta",
            VersionTagType(VersionTag::Prerelease) => "prerelease",
        }
        .to_string()
    }

    pub fn to_string(&self) -> String {
        let mut result = self.to_suffix_string();
        if let Some(number) = self.number {
            result.push_str(&format!(".{}", number));
        }
        result
    }
}

impl VersionTag {
    pub fn parse(tag_str: &str) -> Result<VersionTagWithNumber, ParseIntError> {
        let mut parts = tag_str.split('-');
        let value = parts
            .next()
            .ok_or_else(|| "Invalid version tag")
            .expect("fuck.");
        let number = parts.next().map(|n| n.parse()).transpose()?;
        let value = match value {
            "alpha" => VersionTagType(VersionTag::Alpha),
            "beta" => VersionTagType(VersionTag::Beta),
            "prerelease" => VersionTagType(VersionTag::Prerelease),
            _ => todo!(),
        };
        Ok(VersionTagWithNumber { value, number })
    }

    pub fn to_suffix_string(&self) -> String {
        match self {
            VersionTag::Alpha => "alpha",
            VersionTag::Beta => "beta",
            VersionTag::Prerelease => "prerelease",
        }
        .to_string()
    }
}

impl PartialEq for VersionTagWithNumber {
    fn eq(&self, other: &Self) -> bool {
        self.value.0 == other.value.0 && self.number == other.number
    }
}

impl PartialOrd for VersionTagWithNumber {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        match self.value.0.cmp(&other.value.0) {
            std::cmp::Ordering::Equal => self.number.partial_cmp(&other.number),
            result => Some(result),
        }
    }
}

#[derive(Debug, Eq, Ord, Clone)]
pub struct VersionInfo {
    pub major: usize,
    pub minor: usize,
    pub patch: usize,
    pub tag: Option<VersionTagWithNumber>,
}

impl VersionInfo {
    pub fn new(
        major: usize,
        minor: usize,
        patch: usize,
        tag: Option<VersionTagWithNumber>,
    ) -> Self {
        VersionInfo {
            major,
            minor,
            patch,
            tag,
        }
    }

    pub fn parse(string: &str) -> Result<VersionInfo, &'static str> {
        let mut parts = string.split('.');
        let major = parts.next().ok_or("Invalid version string")?.parse();
        let minor = parts.next().ok_or("Invalid version string")?.parse();
        let patch = parts.next().ok_or("Invalid version string")?.parse();
        let tag_str = parts.next().unwrap_or("");
        let tag = if tag_str.is_empty() {
            None
        } else {
            Some(VersionTag::parse(tag_str))
        };
        Ok(VersionInfo::new(
            major.unwrap(),
            minor.unwrap(),
            patch.unwrap(),
            Some(tag.expect("uwu").unwrap()),
        ))
    }

    pub fn get_major(&self) -> usize {
        self.major
    }

    pub fn get_minor(&self) -> usize {
        self.minor
    }

    pub fn get_patch(&self) -> usize {
        self.patch
    }

    pub fn get_tag(&self) -> Option<&VersionTagWithNumber> {
        self.tag.as_ref()
    }

    pub fn to_string(&self, include_tag: bool) -> String {
        let mut result = format!("{}.{}.{}", self.major, self.minor, self.patch);
        if include_tag {
            if let Some(tag) = &self.tag {
                result.push_str(&format!("-{}", tag.to_string()));
            }
        }
        result
    }
}

impl PartialEq for VersionInfo {
    fn eq(&self, other: &Self) -> bool {
        self.major == other.major
            && self.minor == other.minor
            && self.patch == other.patch
            && self.tag == other.tag
    }
}

impl PartialOrd for VersionInfo {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        match self.major.cmp(&other.major) {
            std::cmp::Ordering::Equal => match self.minor.cmp(&other.minor) {
                std::cmp::Ordering::Equal => match self.patch.cmp(&other.patch) {
                    std::cmp::Ordering::Equal => self.tag.partial_cmp(&other.tag),
                    result => Some(result),
                },
                result => Some(result),
            },
            result => Some(result),
        }
    }
}

#[derive(Debug, Eq, Ord)]
pub struct ComparableVersionInfo {
    pub version: VersionInfo,
    pub compare: VersionCompare,
}

impl ComparableVersionInfo {
    pub fn new(version: VersionInfo, compare: VersionCompare) -> Self {
        ComparableVersionInfo { version, compare }
    }

    pub fn compare(&self, other: &VersionInfo) -> bool {
        self.compare_with_reason(other) == VersionCompareResult::Match
    }

    pub fn compare_with_reason(&self, other: &VersionInfo) -> VersionCompareResult {
        if self.compare == VersionCompare::Any {
            return VersionCompareResult::Match;
        }

        // opposing major versions never match
        if self.version.get_major() != other.get_major() {
            return VersionCompareResult::MajorMismatch;
        }

        // the comparison works invertedly as a version like "v1.2.0"
        // should return true for "<=v1.3.0"
        match self.compare {
            VersionCompare::LessEq => {
                if other <= &self.version {
                    VersionCompareResult::Match
                } else {
                    VersionCompareResult::TooNew
                }
            }
            VersionCompare::MoreEq => {
                if other >= &self.version {
                    VersionCompareResult::Match
                } else {
                    VersionCompareResult::TooOld
                }
            }
            VersionCompare::Less => {
                if other < &self.version {
                    VersionCompareResult::Match
                } else {
                    VersionCompareResult::TooNew
                }
            }
            VersionCompare::More => {
                if other > &self.version {
                    VersionCompareResult::Match
                } else {
                    VersionCompareResult::TooOld
                }
            }
            VersionCompare::Exact => {
                if other == &self.version {
                    VersionCompareResult::Match
                } else if other > &self.version {
                    VersionCompareResult::TooOld
                } else {
                    VersionCompareResult::TooNew
                }
            }
            _ => VersionCompareResult::GenericMismatch,
        }
    }
}

impl PartialEq for ComparableVersionInfo {
    fn eq(&self, other: &Self) -> bool {
        self.version == other.version && self.compare == other.compare
    }
}

impl PartialOrd for ComparableVersionInfo {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        match self.version.partial_cmp(&other.version) {
            Some(std::cmp::Ordering::Equal) => self.compare.partial_cmp(&other.compare),
            result => result,
        }
    }
}

pub fn semver_compare(current: &VersionInfo, target: &VersionInfo) -> bool {
    current < target
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_version_tag_parse() {
        assert_eq!(
            VersionTag::parse("alpha").unwrap(),
            VersionTagWithNumber {
                value: VersionTagType(VersionTag::Alpha),
                number: None
            }
        );
        assert_eq!(
            VersionTag::parse("beta.1").unwrap(),
            VersionTagWithNumber {
                value: VersionTagType(VersionTag::Beta),
                number: Some(1)
            }
        );
        assert_eq!(
            VersionTag::parse("prerelease.42").unwrap(),
            VersionTagWithNumber {
                value: VersionTagType(VersionTag::Prerelease),
                number: Some(42)
            }
        );
        assert!(VersionTag::parse("invalid").is_err());
    }

    #[test]
    fn test_version_info_parse() {
        assert_eq!(
            VersionInfo::parse("1.2.3").unwrap(),
            VersionInfo::new(1, 2, 3, None)
        );
        assert_eq!(
            VersionInfo::parse("2.3.4-beta.1").unwrap(),
            VersionInfo::new(
                2,
                3,
                4,
                Some(VersionTagWithNumber {
                    value: VersionTagType(VersionTag::Beta),
                    number: Some(1)
                })
            )
        );
        assert!(VersionInfo::parse("invalid").is_err());
    }

    #[test]
    fn test_comparable_version_info_compare() {
        let version = VersionInfo::parse("1.2.3").unwrap();
        let comparable_version = ComparableVersionInfo::new(version.clone(), VersionCompare::Exact);

        assert!(comparable_version.compare(&version));
    }

    #[test]
    fn test_semver_compare() {
        let current = VersionInfo::parse("1.2.3").unwrap();
        let target = VersionInfo::parse("2.0.0").unwrap();

        assert!(semver_compare(&current, &target));
    }
}

impl fmt::Display for VersionTagType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0.to_suffix_string())
    }
}

impl fmt::Display for VersionTagWithNumber {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.value)?;
        if let Some(number) = self.number {
            write!(f, ".{}", number)?;
        }
        Ok(())
    }
}

impl fmt::Display for VersionTag {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.to_suffix_string())
    }
}

impl fmt::Display for VersionInfo {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}.{}.{}", self.major, self.minor, self.patch)?;
        if let Some(tag) = &self.tag {
            write!(f, "-{}", tag)?;
        }
        Ok(())
    }
}

impl fmt::Display for ComparableVersionInfo {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{} {}", self.version, self.compare)
    }
}

impl fmt::Display for VersionCompare {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            VersionCompare::LessEq => write!(f, "<="),
            VersionCompare::Exact => write!(f, "=="),
            VersionCompare::MoreEq => write!(f, ">="),
            VersionCompare::Less => write!(f, "<"),
            VersionCompare::More => write!(f, ">"),
            VersionCompare::Any => write!(f, "any"),
        }
    }
}
