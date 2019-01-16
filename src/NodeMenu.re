open Definition;
open AppActions;

/* type category =
   | ReferenceCategory
   | ListCategory
   | ConstantCategory
   | FunctionCategory
   | FunctionInterfaceCategory
   | RecordCategory; */

/* type state = {category}; */

/* type action =
   | SelectCategory(category); */

let component = ReasonReact.statelessComponent("NodeMenu");
let make =
    (
      ~definitions: definitions,
      ~nib: explicitConnectionSide,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  /* initialState: () => {category: ReferenceCategory},
     reducer: (action: action, state: state) =>
       switch (action) {
       | SelectCategory(category) => ReasonReact.Update({category: category})
       }, */
  render: self => {
    <div>
      <a
        onClick={_event =>
          emit(
            AddNode({
              node: ReferenceNode,
              explicitConnectionSide: nib,
              connectionNib: ValueConnection,
            }),
          )
        }>
        {ReasonReact.string("Reference")}
      </a>
    </div>;
  },
};
