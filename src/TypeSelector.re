open Definition;

type state = {opened: bool};

type action =
  | Toggle;

let component = ReasonReact.reducerComponent("TypeSelector");

let make =
    (~valueType: valueType, ~definitions: definitions, ~changeType, _children) => {
  ...component,
  initialState: () => {opened: false},
  reducer: (action: action, state: state) =>
    switch (action) {
    | Toggle => ReasonReact.Update({opened: !state.opened})
    },
  render: self => {
    <div>
      <a onClick={_event => self.send(Toggle)}>
        {ReasonReact.string(displayValueType(valueType, definitions, "en"))}
      </a>
      {self.state.opened ?
         <div>
           <a onClick={_event => changeType(PrimitiveValueType(TextType))}>
             {ReasonReact.string("Text")}
           </a>
           <a onClick={_event => changeType(PrimitiveValueType(NumberType))}>
             {ReasonReact.string("Number")}
           </a>
         </div> :
         ReasonReact.null}
    </div>;
  },
};
