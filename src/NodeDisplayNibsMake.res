let f = (
  ~inputs=list{},
  ~outputs=list{},
  ~internalInputs=list{},
  ~internalOutputs=list{},
  _unit: unit,
) => {
  open NodeDisplayNibs
  {
    inputs: inputs,
    outputs: outputs,
    internalInputs: internalInputs,
    internalOutputs: internalOutputs,
  }
}
