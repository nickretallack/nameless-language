let f = (nib: ConnectionNib.t): bool =>
  switch (nib) {
  | NibConnection(_) => true
  | _ => false
  };
