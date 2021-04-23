let f = (interface: Interface.t, nibID: NibID.t, isInput: bool): Interface.t =>
  InterfaceEditNib.f(interface, isInput, fields =>
    Belt.Map.String.keep(fields, (interfaceNibID, _) => interfaceNibID != nibID)
  )
