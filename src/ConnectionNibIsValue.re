let f = (nib: ConnectionNib.t): bool =>
  switch (nib) {
  | ValueConnection => true
  | _ => false
  };
