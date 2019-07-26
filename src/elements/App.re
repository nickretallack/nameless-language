[%%debugger.chrome];
open Definition;
open! AppActions;
open Helpers;
open! Evaluate;
open! AppReducer;

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
    error: NoAppError,
    execution: None,
  },
  didMount: self => {
    let watcherId =
      ReasonReact.Router.watchUrl(url => self.send(ChangeRoute(url)));
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherId));
  },
  reducer: AppReducer.reducer,
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
               definitionID
               definitions={self.state.definitions}
               definition
               implementation
               display
               documentation
               emit
               error={self.state.error}
             />
           | _ =>
             <SimpleDefinition
               definitionID
               definition
               definitions
               emit
               error={self.state.error}
             />
           };
         };
       }}
      {switch (self.state.execution) {
       | None => ReasonReact.null
       | Some(_) =>
         <div>
           <button onClick={_ => self.send(Step)}>
             {ReasonReact.string("step")}
           </button>
         </div>
       }}
    </div>,
};
