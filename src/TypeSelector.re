open Definition;

let component = ReasonReact.statelessComponent("TypeSelector");

let make =
    (~valueType: valueType, ~definitions: definitions, ~changeType, _children) => {
  ...component,
  render: _self => {
    <div>
      <div>
        {ReasonReact.string(displayValueType(valueType, definitions, "en"))}
      </div>
      <a onClick={_event => changeType(PrimitiveValueType(TextType))}>
        {ReasonReact.string("Text")}
      </a>
      <a onClick={_event => changeType(PrimitiveValueType(NumberType))}>
        {ReasonReact.string("Number")}
      </a>
    </div>;
  },
};
