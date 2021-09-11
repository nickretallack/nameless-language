type t =
  | Final(HashedContent.t)
  | Recursion
  | MutualRecursion(Definition.t)
