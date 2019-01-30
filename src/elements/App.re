[%%debugger.chrome];
open Definition;
open! AppActions;
open Helpers;

type appState = {
  definitions,
  definitionID,
};

let makers = [|
  ("Function", () => makeGraph(~outputs=[|(randomID(), "")|], ())),
  (
    "Constant",
    () =>
      makeDefinition(
        ~implementation=ConstantImplementation(TextValue("")),
        (),
      ),
  ),
  (
    "Interface",
    () => {
      let outputID = randomID();
      makeDefinition(
        ~outputs=[|(outputID, "")|],
        ~implementation=
          InterfaceImplementation({
            inputTypes: Belt.Map.String.empty,
            outputTypes:
              Belt.Map.String.fromArray([|
                (outputID, PrimitiveValueType(TextType)),
              |]),
          }),
        (),
      );
    },
  ),
  (
    "Record",
    () => {
      let inputID1 = randomID();
      let inputID2 = randomID();
      makeDefinition(
        ~inputs=[|(inputID1, ""), (inputID2, "")|],
        ~implementation=
          RecordTypeImplementation(
            Belt.Map.String.fromArray([|
              (inputID1, PrimitiveValueType(TextType)),
              (inputID2, PrimitiveValueType(TextType)),
            |]),
          ),
        (),
      );
    },
  ),
|];

let component = ReasonReact.reducerComponent("App");

let make = (~definitions, _children) => {
  ...component,
  initialState: () => {
    definitions,
    definitionID: ReasonReact.Router.dangerouslyGetInitialUrl().hash,
  },
  didMount: self => {
    let watcherId =
      ReasonReact.Router.watchUrl(url => self.send(ChangeRoute(url)));
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherId));
  },
  reducer: (action: appAction, state: appState) =>
    switch (action) {
    | ChangeRoute(url) =>
      ReasonReact.Update({...state, definitionID: url.hash})
    | CreateDefinition(definition) =>
      let definitionID = randomID();
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          definitions:
            Belt.Map.String.set(state.definitions, definitionID, definition),
        },
        _ => ReasonReact.Router.push("#" ++ definitionID),
      );
    | DefinitionAction({definitionID, action}) =>
      let definition =
        Belt.Map.String.getExn(state.definitions, definitionID);
      let newDefinition =
        switch (action) {
        | CreateConnection({source, sink}) =>
          switch (definition.implementation) {
          | GraphImplementation(graphImplementation) => {
              ...definition,
              implementation:
                GraphImplementation({
                  ...graphImplementation,
                  connections:
                    Belt.Map.set(
                      graphImplementation.connections,
                      sink,
                      source,
                    ),
                }),
            }
          | _ => definition
          }

        | AddNode({node, explicitConnectionSide, connectionNib}) =>
          let nodeID = randomID();
          let nodeConnectionSide: connectionSide = {
            node: NodeConnection(nodeID),
            nib: connectionNib,
          };
          let (source, sink) =
            explicitConnectionSide.isSource ?
              (explicitConnectionSide.connectionSide, nodeConnectionSide) :
              (nodeConnectionSide, explicitConnectionSide.connectionSide);
          switch (definition.implementation) {
          | GraphImplementation(graphImplementation) => {
              ...definition,
              implementation:
                GraphImplementation({
                  nodes:
                    Belt.Map.String.set(
                      graphImplementation.nodes,
                      nodeID,
                      node,
                    ),
                  connections:
                    Belt.Map.set(
                      graphImplementation.connections,
                      sink,
                      source,
                    ),
                }),
            }
          | _ => definition
          };
        | ChangeConstantValue(implementation) =>
          switch (definition.implementation) {
          | ConstantImplementation(_) => {
              ...definition,
              implementation: ConstantImplementation(implementation),
            }
          | _ => raise(Not_found)
          }
        | ChangeName(text) => {
            ...definition,
            documentation: {
              ...definition.documentation,
              name: setTranslated(definition.documentation.name, "en", text),
            },
          }
        | ChangeDescription(text) => {
            ...definition,
            documentation: {
              ...definition.documentation,
              name:
                setTranslated(
                  definition.documentation.description,
                  "en",
                  text,
                ),
            },
          }
        | AddInput =>
          let nibID = randomID();
          {
            documentation: {
              ...definition.documentation,
              inputs:
                Belt.Map.String.set(
                  definition.documentation.inputs,
                  nibID,
                  emptyTranslatable,
                ),
            },
            display: {
              ...definition.display,
              inputOrdering:
                List.append(definition.display.inputOrdering, [nibID]),
            },
            implementation:
              switch (definition.implementation) {
              | InterfaceImplementation(interface) =>
                InterfaceImplementation({
                  ...interface,
                  inputTypes:
                    Belt.Map.String.set(
                      interface.inputTypes,
                      nibID,
                      PrimitiveValueType(TextType),
                    ),
                })
              | RecordTypeImplementation(typedFields) =>
                RecordTypeImplementation(
                  Belt.Map.String.set(
                    typedFields,
                    nibID,
                    PrimitiveValueType(TextType),
                  ),
                )
              | _ => definition.implementation
              },
          };
        | AddOutput =>
          let nibID = randomID();
          {
            documentation: {
              ...definition.documentation,
              outputs:
                Belt.Map.String.set(
                  definition.documentation.outputs,
                  nibID,
                  emptyTranslatable,
                ),
            },
            display: {
              ...definition.display,
              outputOrdering:
                List.append(definition.display.outputOrdering, [nibID]),
            },
            implementation:
              switch (definition.implementation) {
              | InterfaceImplementation(interface) =>
                InterfaceImplementation({
                  ...interface,
                  outputTypes:
                    Belt.Map.String.set(
                      interface.outputTypes,
                      nibID,
                      PrimitiveValueType(NumberType),
                    ),
                })
              | _ => definition.implementation
              },
          };
        | NibAction({nibID, isInput, action}) =>
          switch (action) {
          | ChangeNibName(text) =>
            let nibs =
              isInput ?
                definition.documentation.inputs :
                definition.documentation.outputs;
            let nib = Belt.Map.String.getExn(nibs, nibID);
            let newNib = setTranslated(nib, "en", text);
            let newNibs = Belt.Map.String.set(nibs, nibID, newNib);
            let documentation =
              isInput ?
                {...definition.documentation, inputs: newNibs} :
                {...definition.documentation, outputs: newNibs};
            {...definition, documentation};
          | ChangeNibType(valueType) => {
              ...definition,
              implementation:
                switch (definition.implementation) {
                | InterfaceImplementation(interface) =>
                  InterfaceImplementation(
                    isInput ?
                      {
                        ...interface,
                        inputTypes:
                          changeTypedFields(
                            interface.inputTypes,
                            nibID,
                            valueType,
                          ),
                      } :
                      {
                        ...interface,
                        outputTypes:
                          changeTypedFields(
                            interface.outputTypes,
                            nibID,
                            valueType,
                          ),
                      },
                  )
                | RecordTypeImplementation(typedFields) =>
                  RecordTypeImplementation(
                    isInput ?
                      changeTypedFields(typedFields, nibID, valueType) :
                      raise(Not_found),
                  )
                | _ => raise(Not_found)
                },
            }
          | ChangeNibOrdering(index) => {
              ...definition,
              display:
                isInput ?
                  {
                    ...definition.display,
                    inputOrdering:
                      moveToListIndex(
                        definition.display.inputOrdering,
                        nibID,
                        index,
                      ),
                  } :
                  {
                    ...definition.display,
                    outputOrdering:
                      moveToListIndex(
                        definition.display.outputOrdering,
                        nibID,
                        index,
                      ),
                  },
            }
          }
        };
      ReasonReact.Update({
        ...state,
        definitions:
          Belt.Map.String.set(state.definitions, definitionID, newDefinition),
      });
    },
  render: self =>
    <div>
      <a href="#"> {ReasonReact.string("Library")} </a>
      {ReasonReact.string(" New:")}
      {ReasonReact.array(
         Belt.Array.mapWithIndex(makers, (index, (name, maker)) =>
           <a
             className="maker"
             key={string_of_int(index)}
             onClick={_event => self.send(CreateDefinition(maker()))}>
             {ReasonReact.string(name)}
           </a>
         ),
       )}
      {switch (self.state.definitionID) {
       | "" => <DefinitionList definitions={self.state.definitions} />
       | _ =>
         let definitionID = self.state.definitionID;
         switch (Belt.Map.String.get(self.state.definitions, definitionID)) {
         | None => ReasonReact.string("Not found")
         | Some(definition) =>
           let {implementation, display, documentation} = definition;
           let emit = (action: definitionAction) =>
             self.send(DefinitionAction({definitionID, action}));
           switch (implementation) {
           | GraphImplementation(implementation) =>
             <Graph
               key=definitionID
               definitions={self.state.definitions}
               definition
               implementation
               display
               documentation
               emit
             />
           | _ => <SimpleDefinition definition definitions emit />
           };
         };
       }}
    </div>,
};
