let f =
    (interface: Interface.t, isInput: bool, mutation: Record.t => Record.t) =>
  isInput
    ? {...interface, input: mutation(interface.input)}
    : {...interface, output: mutation(interface.output)};
