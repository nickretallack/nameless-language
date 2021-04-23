type t =
  | ChangeNibName(string)
  | ChangeNibType(ValueType.t)
  | ChangeNibOrdering(int)
  | RemoveNib
