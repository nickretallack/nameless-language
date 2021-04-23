let f = (connectionNib: ConnectionNib.t): string =>
  switch connectionNib {
  | ValueConnection => "value"
  | NibConnection(nibID) => "nib-" ++ nibID
  | PositionalConnection(index) => "index-" ++ string_of_int(index)
  }
