type t =
  | ValueConnection
  | NibConnection(NibID.t)
  | PositionalConnection(int);
