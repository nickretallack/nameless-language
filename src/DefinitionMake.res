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
      inputOrdering: Array.map(((id, _name)) => id, inputs),
      outputOrdering: Array.map(((id, _name)) => id, outputs),
    },
    implementation: implementation,
  }
}
