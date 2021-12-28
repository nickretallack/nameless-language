type t = {
  definitionID: DefinitionID.t,
  inputs: SourceValues.t,
  connectionNib: ConnectionNib.t,
  inlineFunctionContext: InlineFunctionContext.t,
  callingContext: option<CallingContext.t>,
}
