let f =
    (
      {nibID, isInput, action}: NibActionRecord.t,
      state: AppState.t,
      definitionID: DefinitionID.t,
      definition: Definition.t,
      updateDefinition:
        Definition.t => ReactUpdate.update(AppAction.t, AppState.t),
    )
    : ReactUpdate.update(AppAction.t, AppState.t) => {
  switch (action) {
  | ChangeNibName(text) =>
    let nibs =
      isInput
        ? definition.documentation.inputs : definition.documentation.outputs;
    let nib = Belt.Map.String.getExn(nibs, nibID);
    let newNib = TranslatableSetText.f(nib, state.languageName, text);
    let newNibs = Belt.Map.String.set(nibs, nibID, newNib);
    let documentation =
      isInput
        ? {...definition.documentation, inputs: newNibs}
        : {...definition.documentation, outputs: newNibs};
    updateDefinition({...definition, documentation});
  | ChangeNibType(valueType) =>
    updateDefinition({
      ...definition,
      implementation:
        switch (definition.implementation) {
        | InterfaceImplementation(interface) =>
          InterfaceImplementation(
            InterfaceChange.f(interface, isInput, nibID, valueType),
          )
        | RecordTypeImplementation(typedFields) =>
          RecordTypeImplementation(
            isInput
              ? RecordChange.f(typedFields, nibID, valueType)
              : raise(Not_found),
          )
        | GraphImplementation(graphImplementation) =>
          GraphImplementation({
            ...graphImplementation,
            interface:
              InterfaceChange.f(
                graphImplementation.interface,
                isInput,
                nibID,
                valueType,
              ),
          })
        | _ => raise(Not_found)
        },
    })
  | ChangeNibOrdering(index) =>
    updateDefinition({
      ...definition,
      display:
        isInput
          ? {
            ...definition.display,
            inputOrdering:
              ListMoveItemToIndex.f(
                definition.display.inputOrdering,
                nibID,
                index,
              ),
          }
          : {
            ...definition.display,
            outputOrdering:
              ListMoveItemToIndex.f(
                definition.display.outputOrdering,
                nibID,
                index,
              ),
          },
    })
  | RemoveNib =>
    let uses =
      FindConnectedDefinitions.f(
        definitionID,
        nibID,
        isInput,
        state.definitions,
      );
    if (Belt.Map.String.isEmpty(uses)) {
      /* TODO: remove nib from translations?  Unify "interface" usage? */
      updateDefinition({
        ...definition,
        display:
          isInput
            ? {
              ...definition.display,
              inputOrdering:
                Belt.List.keep(definition.display.inputOrdering, item =>
                  item != nibID
                ),
            }
            : {
              ...definition.display,
              outputOrdering:
                Belt.List.keep(definition.display.outputOrdering, item =>
                  item != nibID
                ),
            },
        implementation:
          switch (definition.implementation) {
          | GraphImplementation(graphImplementation) =>
            GraphImplementation({
              ...graphImplementation,
              connections:
                Belt.Map.keep(graphImplementation.connections, (sink, source) =>
                  !(
                    switch (isInput ? source : sink) {
                    | {
                        node: GraphConnection,
                        nib: NibConnection(connectionNibID),
                      } =>
                      connectionNibID == nibID
                    | _ => false
                    }
                  )
                ),
              interface:
                InterfaceRemoveNib.f(
                  graphImplementation.interface,
                  nibID,
                  isInput,
                ),
            })
          | InterfaceImplementation(interface) =>
            InterfaceImplementation(
              InterfaceRemoveNib.f(interface, nibID, isInput),
            )
          | RecordTypeImplementation(typedFields) =>
            RecordTypeImplementation(
              Belt.Map.String.keep(typedFields, (fieldNibID, _) =>
                fieldNibID != nibID
              ),
            )
          | other => other
          },
      });
    } else {
      ReactUpdate.Update(
        {
          {...state, error: NibIsConnected(uses)};
        },
      );
    };
  };
};
