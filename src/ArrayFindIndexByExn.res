let f: 'a. (array<'a>, 'a => bool) => int = (array, check) =>
  switch Belt.Array.getIndexBy(array, check) {
  | Some(index) => index
  | None => raise(Not_found)
  }
