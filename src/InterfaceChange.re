let f =
    (
      interface: Interface.t,
      isInput: bool,
      nibID: NibID.t,
      valueType: ValueType.t,
    )
    : Interface.t =>
  isInput
    ? {
      ...interface,
      input: RecordChange.f(interface.input, nibID, valueType),
    }
    : {
      ...interface,
      output: RecordChange.f(interface.output, nibID, valueType),
    };
