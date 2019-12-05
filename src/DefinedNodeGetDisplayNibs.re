let f =
    (
      definition: Definition.t,
      kind: DefinedNodeKind.t,
      language: LanguageName.t,
    )
    : NodeDisplayNibs.t =>
  switch (kind) {
  | FunctionCallNode =>
    NodeDisplayNibsMake.f(
      ~inputs=DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      ~outputs=DefinitionGetKeywordDisplayNibOutputs.f(definition, language),
      (),
    )
  | ValueNode =>
    NodeDisplayNibsMake.f(~outputs=[{nib: ValueConnection, name: ""}], ())
  | FunctionPointerCallNode =>
    NodeDisplayNibsMake.f(
      ~inputs=[
        {nib: ValueConnection, name: "implementation"},
        ...DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      ],
      ~outputs=DefinitionGetKeywordDisplayNibOutputs.f(definition, language),
      (),
    )
  | FunctionDefinitionNode =>
    NodeDisplayNibsMake.f(
      ~outputs=[{nib: ValueConnection, name: ""}],
      ~internalInputs=
        DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      ~internalOutputs=
        DefinitionGetKeywordDisplayNibOutputs.f(definition, language),
      (),
    )
  | ConstructorNode =>
    NodeDisplayNibsMake.f(
      ~inputs=DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      ~outputs=[{nib: ValueConnection, name: ""}],
      (),
    )
  | AccessorNode =>
    NodeDisplayNibsMake.f(
      ~inputs=[{nib: ValueConnection, name: ""}],
      ~outputs=DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      (),
    )
  };
