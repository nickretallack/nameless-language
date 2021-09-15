type t =
  | Final(HashedContent.t)
  | Recursion
  | UnorderedMutualRecursion
  | OrderedMutualRecursion(int)
  | FinalMutualRecursion(MutualRecursion.t)
