let f: 'a. (array<'a>, int, 'a) => 'a = (array, index, default) =>
  switch Belt.Array.get(array, index) {
  | None => default
  | Some(value) => value
  }
