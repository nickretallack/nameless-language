let f = (
  languageName: LanguageName.t,
  ~name="",
  ~description="",
  ~inputs=[],
  ~outputs=[],
  ~implementation,
  _unit,
) => {
  open Definition
  {
    documentation: {
      name: TranslatableMake.f(name, languageName),
      description: TranslatableMake.f(description, languageName),
      inputs: RecordDocumentationMake.f(inputs),
      outputs: RecordDocumentationMake.f(outputs),
    },
    display: {
      inputOrdering: Array.to_list(Array.map(((id, _name)) => id, inputs)),
      outputOrdering: Array.to_list(Array.map(((id, _name)) => id, outputs)),
    },
    implementation: implementation,
  }
}
