let f: 'a. (array<'a>, 'a) => int = (array, needle) =>
  switch Belt.Array.getIndexBy(array, item => item == needle) {
  | Some(index) => index
  | None => raise(Not_found)
  }
