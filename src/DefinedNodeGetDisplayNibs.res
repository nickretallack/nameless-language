let f = (
  definition: Definition.t,
  kind: DefinedNodeKind.t,
  language: LanguageName.t,
): NodeDisplayNibs.t =>
  switch kind {
  | FunctionCallNode =>
    NodeDisplayNibsMake.f(
      ~inputs=DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      ~outputs=DefinitionGetKeywordDisplayNibOutputs.f(definition, language),
      (),
    )
  | ValueNode => NodeDisplayNibsMake.f(~outputs=list{{nib: ValueConnection, name: ""}}, ())
  | FunctionPointerCallNode =>
    NodeDisplayNibsMake.f(
      ~inputs=list{
        {nib: ValueConnection, name: "implementation"},
        ...DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      },
      ~outputs=DefinitionGetKeywordDisplayNibOutputs.f(definition, language),
      (),
    )
  | FunctionDefinitionNode =>
    NodeDisplayNibsMake.f(
      ~outputs=list{{nib: ValueConnection, name: ""}},
      ~internalInputs=DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      ~internalOutputs=DefinitionGetKeywordDisplayNibOutputs.f(definition, language),
      (),
    )
  | ConstructorNode =>
    NodeDisplayNibsMake.f(
      ~inputs=DefinitionIsWrappedType.f(definition)
        ? list{{nib: ValueConnection, name: "wrapped"}}
        : DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      ~outputs=list{{nib: ValueConnection, name: ""}},
      (),
    )
  | AccessorNode =>
    NodeDisplayNibsMake.f(
      ~inputs=list{{nib: ValueConnection, name: ""}},
      ~outputs=DefinitionIsWrappedType.f(definition)
        ? list{{nib: ValueConnection, name: "wrapped"}}
        : DefinitionGetKeywordDisplayNibInputs.f(definition, language),
      (),
    )
  }
