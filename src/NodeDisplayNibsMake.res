let f = (~inputs=[], ~outputs=[], ~internalInputs=[], ~internalOutputs=[], _unit: unit) => {
  open NodeDisplayNibs
  {
    inputs: inputs,
    outputs: outputs,
    internalInputs: internalInputs,
    internalOutputs: internalOutputs,
  }
}
