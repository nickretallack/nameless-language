let f =
    (
      ~name="",
      ~description="",
      ~inputs=[||],
      ~outputs=[||],
      ~implementation,
      _unit,
    ) =>
  Definition.{
    documentation: {
      name: TranslatableMake.f(name, "en"),
      description: TranslatableMake.f(description, "en"),
      inputs: RecordDocumentationMake.f(inputs),
      outputs: RecordDocumentationMake.f(outputs),
    },
    display: {
      inputOrdering: Array.to_list(Array.map(((id, _name)) => id, inputs)),
      outputOrdering:
        Array.to_list(Array.map(((id, _name)) => id, outputs)),
    },
    implementation,
  };
