let f = (interface: Interface.t, isInput: bool, mutation: RecordType.t => RecordType.t) =>
  isInput
    ? {...interface, input: mutation(interface.input)}
    : {...interface, output: mutation(interface.output)}
