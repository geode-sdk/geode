use chrono::{TimeZone, Utc};

pub fn time_point_as_string(tp: chrono::DateTime<Utc>) -> String {
    let t = tp.timestamp();
    let buf = Utc.timestamp_opt(t, 0).unwrap().to_rfc2822();
    let mut res = buf.to_string();
    res.pop();
    res
}
