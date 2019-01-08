[%%debugger.chrome];
open Definition;
open AppActions;
open Helpers;

type appState = {
  definitions,
  definitionID,
};

let component = ReasonReact.reducerComponent("App");

let make = (~size, ~definitions, _children) => {
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
          let nibID = randomId();
          {
            ...definition,
            documentation: {
              ...definition.documentation,
              inputs:
                Belt.Map.String.set(
                  definition.documentation.inputs,
                  nibID,
                  emptyTranslatable,
                ),
              outputs:
                Belt.Map.String.set(
                  definition.documentation.inputs,
                  nibID,
                  emptyTranslatable,
                ),
            },
            display: {
              inputOrdering:
                List.append(definition.display.inputOrdering, [nibID]),
              outputOrdering:
                List.append(definition.display.outputOrdering, [nibID]),
            },
          };
        };
      ReasonReact.Update({
        ...state,
        definitions:
          Belt.Map.String.set(state.definitions, definitionID, newDefinition),
      });
    },
  render: self =>
    switch (self.state.definitionID) {
    | "" => <DefinitionList definitions={self.state.definitions} />
    | _ =>
      switch (
        Belt.Map.String.get(self.state.definitions, self.state.definitionID)
      ) {
      | None => ReasonReact.string("Not found")
      | Some({implementation, display, documentation}) =>
        switch (implementation) {
        | GraphImplementation(implementation) =>
          <Graph
            definitionID={self.state.definitionID}
            definitions={self.state.definitions}
            implementation
            display
            documentation
            size
            emit={action => self.send(action)}
          />
        | _ => ReasonReact.string("TODO")
        }
      }
    },
};
