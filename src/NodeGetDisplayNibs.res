let f = (node: Node.t, definitions: DefinitionMap.t, language: LanguageName.t): NodeDisplayNibs.t =>
  switch node.kind {
  | ListNode(length) =>
    NodeDisplayNibsMake.f(
      ~outputs=list{{nib: ValueConnection, name: ""}},
      ~inputs=Belt.List.makeBy(length, index => {
        open DisplayNib
        {
          nib: PositionalConnection(index),
          name: string_of_int(index),
        }
      }),
      (),
    )
  | DefinedNode({kind, definitionID}) =>
    let definition = Belt.Map.String.getExn(definitions, definitionID)
    DefinedNodeGetDisplayNibs.f(definition, kind, language)
  }
