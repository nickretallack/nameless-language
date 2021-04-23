module C = Belt.Id.MakeComparable({
  type t = ConnectionSide.t
  let cmp = compare
})
