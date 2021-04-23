let f: 'a. (list<'a>, 'a, int) => list<'a> = (list, needle, newIndex) => {
  let oldIndex = ListFindIndexExn.f(list, needle)
  let (left, right) = switch Belt.List.splitAt(list, newIndex + (newIndex > oldIndex ? 1 : 0)) {
  | None => raise(Not_found)
  | Some(tuple) => tuple
  }
  Belt.List.concat(ListRemoveItem.f(left, needle), list{needle, ...ListRemoveItem.f(right, needle)})
}
