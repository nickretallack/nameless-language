module C = Belt.Id.MakeComparable({
  type t = ScheduledEventIdentifier.t
  let cmp = compare
})

type t = Belt.Set.t<ScheduledEventIdentifier.t, C.identity>

let fromArray = array => Belt.Set.fromArray(array, ~id=module(C))
