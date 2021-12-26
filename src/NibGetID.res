let f = (nib: ConnectionNib.t) =>
  switch nib {
  | NibConnection(nibID) => nibID
  | _ => raise(Exception.ShouldntHappen("add scheduled event must use nib connections"))
  }
