[@react.component]
let make = (~initialDefinitions) => {
  let (state, dispatch) =
    ReactUpdate.useReducer(
      AppState.{
        definitions: initialDefinitions,
        error: NoAppError,
        execution: None,
        languageName: "en",
      },
      AppReducer.f,
    );
  let url = ReasonReactRouter.useUrl();
  let definitionID = url.hash;
  let AppState.{languageName, definitions, error, execution} = state;
  <div>
    <a href="#"> {ReasonReact.string("Library")} </a>
    {ReasonReact.string(" New:")}
    {ReasonReact.array(
       Belt.Array.mapWithIndex(DefinitionMakers.v, (index, (name, maker)) =>
         <a
           className="maker"
           key={string_of_int(index)}
           onClick={_event => {
             let newDefinitionID = RandomIDMake.f();
             dispatch(AppAction.CreateDefinition(maker()));
             ReasonReactRouter.push("#" ++ newDefinitionID);
           }}>
           {ReasonReact.string(name)}
         </a>
       ),
     )}
    {switch (definitionID) {
     | "" => <DefinitionListView definitions languageName />
     | _ =>
       // debug
       switch (execution) {
       | None => ()
       | Some(execution) =>
         Js.log(
           Belt.List.toArray(
             Belt.List.reverse(
               Belt.List.map(execution.stack, frame =>
                 ExplicitConnectionSideKey.f(frame.explicitConnectionSide)
               ),
             ),
           ),
         )
       };
       switch (Belt.Map.String.get(definitions, definitionID)) {
       | None => ReasonReact.string("Not found")
       | Some(definition) =>
         let Definition.{implementation, display, documentation} = definition;
         let emit = (action: DefinitionAction.t) =>
           dispatch(DefinitionAction({definitionID, action}));
         let stackFrame =
           switch (execution) {
           | Some(execution) =>
             let StackFrame.{scopeID, explicitConnectionSide, action} =
               Belt.List.headExn(execution.stack);
             let scope = Belt.Map.String.getExn(execution.scopes, scopeID);
             Some(
               MaterializedStackFrame.{scope, explicitConnectionSide, action},
             );
           | None => None
           };
         switch (implementation) {
         | GraphImplementation(implementation) =>
           <GraphView
             key=definitionID
             definitionID
             definitions
             definition
             implementation
             display
             documentation
             emit
             error
             stackFrame
             languageName
           />
         | _ =>
           <SimpleDefinitionView
             definitionID
             definition
             definitions
             languageName
             emit
             error
           />
         };
       };
     }}
    {switch (execution) {
     | None => ReasonReact.null
     | Some(execution) =>
       <div>
         <button onClick={_ => dispatch(Step)}>
           {ReasonReact.string("step")}
         </button>
         {switch (execution.result) {
          | None => ReasonReact.null
          | Some(value) =>
            ReasonReact.string(
              "Result: " ++ ValueDisplay.f(value, definitions, languageName),
            )
          }}
       </div>
     }}
  </div>;
};
