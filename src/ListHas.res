let f: 'a. (list<'a>, 'a) => bool = (haystack, needle) =>
  Belt.List.has(haystack, needle, (a, b) => a == b)
