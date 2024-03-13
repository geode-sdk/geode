use std::cmp::Ordering;
use std::fmt;
use std::num::ParseIntError;
use std::str::FromStr;

#[derive(Debug, PartialEq, PartialOrd, Eq, Ord)]
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

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum VersionTag {
    Alpha,
    Beta,
    Prerelease,
}

impl FromStr for VersionTag {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "Alpha" => Ok(VersionTag::Alpha),
            "Beta" => Ok(VersionTag::Beta),
            "Prerelease" => Ok(VersionTag::Prerelease),
            _ => Err(()),
        }
    }
}

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct VersionInfo {
    pub major: usize,
    pub minor: usize,
    pub patch: usize,
    pub tag: Option<(VersionTag, Option<usize>)>,
}

impl VersionInfo {
    pub fn new(
        major: usize,
        minor: usize,
        patch: usize,
        tag: Option<(VersionTag, Option<usize>)>,
    ) -> Self {
        VersionInfo {
            major,
            minor,
            patch,
            tag,
        }
    }

    pub fn parse(string: &str) -> Result<Self, ParseIntError> {
        // Parse the string to extract version components
        let parts: Vec<&str> = string.split('.').collect();
        let major = parts[0].parse()?;
        let minor = parts[1].parse()?;
        let patch = parts[2].parse()?;

        // Parse the tag if it exists
        let tag = if parts.len() > 3 {
            let tag_parts: Vec<&str> = parts[3].split('-').collect();
            let tag_name = VersionTag::from_str(tag_parts[0]).unwrap();
            let tag_number = if tag_parts.len() > 1 {
                Some(tag_parts[1].parse().unwrap())
            } else {
                None
            };
            Some((tag_name, tag_number))
        } else {
            None
        };

        Ok(VersionInfo::new(major, minor, patch, tag))
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

    pub fn get_tag(&self) -> Option<&(VersionTag, Option<usize>)> {
        self.tag.as_ref()
    }
}

impl fmt::Display for VersionInfo {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}.{}.{}", self.major, self.minor, self.patch)?;
        if let Some((tag, number)) = &self.tag {
            write!(f, "-{:?}", tag)?;
            if let Some(num) = number {
                write!(f, ".{}", num)?;
            }
        }
        Ok(())
    }
}

#[derive(Debug, Eq)]
pub struct ComparableVersionInfo {
    version: VersionInfo,
    compare: VersionCompare,
}

impl ComparableVersionInfo {
    pub fn new(version: VersionInfo, compare: VersionCompare) -> Self {
        ComparableVersionInfo { version, compare }
    }

    pub fn parse(string: &str) -> Result<Self, ParseIntError> {
        let parts: Vec<&str> = string.split(' ').collect();
        let version = VersionInfo::parse(parts[0])?;
        let compare = match parts[1] {
            "<=" => VersionCompare::LessEq,
            "==" => VersionCompare::Exact,
            ">=" => VersionCompare::MoreEq,
            "<" => VersionCompare::Less,
            ">" => VersionCompare::More,
            _ => VersionCompare::Any,
        };
        Ok(ComparableVersionInfo::new(version, compare))
    }

    pub fn compare(&self, version: &VersionInfo) -> VersionCompareResult {
        match self.compare {
            VersionCompare::Any => VersionCompareResult::Match,
            _ => {
                // opposing major versions never match
                if self.version.get_major() != version.get_major() {
                    return VersionCompareResult::MajorMismatch;
                }

                match self.compare {
                    VersionCompare::LessEq => {
                        if version <= &self.version {
                            VersionCompareResult::Match
                        } else {
                            VersionCompareResult::TooNew
                        }
                    }
                    VersionCompare::MoreEq => {
                        if version >= &self.version {
                            VersionCompareResult::Match
                        } else {
                            VersionCompareResult::TooOld
                        }
                    }
                    VersionCompare::Less => {
                        if version < &self.version {
                            VersionCompareResult::Match
                        } else {
                            VersionCompareResult::TooNew
                        }
                    }
                    VersionCompare::More => {
                        if version > &self.version {
                            VersionCompareResult::Match
                        } else {
                            VersionCompareResult::TooOld
                        }
                    }
                    VersionCompare::Exact => {
                        if version == &self.version {
                            VersionCompareResult::Match
                        } else if version > &self.version {
                            VersionCompareResult::TooOld
                        } else {
                            VersionCompareResult::TooNew
                        }
                    }
                    VersionCompare::Any => VersionCompareResult::GenericMismatch,
                }
            }
        }
    }
}

impl fmt::Display for ComparableVersionInfo {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{} {}",
            self.version,
            match self.compare {
                VersionCompare::LessEq => "<=",
                VersionCompare::Exact => "==",
                VersionCompare::MoreEq => ">=",
                VersionCompare::Less => "<",
                VersionCompare::More => ">",
                VersionCompare::Any => "*",
            }
        )
    }
}

impl FromStr for ComparableVersionInfo {
    type Err = ParseIntError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        ComparableVersionInfo::parse(s)
    }
}

impl PartialEq for ComparableVersionInfo {
    fn eq(&self, other: &Self) -> bool {
        self.version == other.version && self.compare == other.compare
    }
}

impl PartialOrd for ComparableVersionInfo {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for ComparableVersionInfo {
    fn cmp(&self, other: &Self) -> Ordering {
        (&self.version, &self.compare).cmp(&(&other.version, &other.compare))
    }
}
