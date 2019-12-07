[@react.component]
let make =
    (~valueType: ValueType.t, ~definitions: DefinitionMap.t, ~changeType) => {
  let (state, dispatch) =
    React.useReducer(
      TypeSelectorReducer.f,
      {opened: false, category: TypeToCategory.f(valueType, definitions)},
    );

  let renderCategory = (name: string, category: TypeSelectorCategory.t) =>
    <a
      onClick={_event => {
        dispatch(SelectCategory(category));
        switch (category) {
        | TextCategory => changeType(ValueType.PrimitiveValueType(TextType))
        | NumberCategory => changeType(PrimitiveValueType(NumberType))
        | _ => ()
        };
      }}
      className={state.category == category ? "selected" : ""}>
      {ReasonReact.string(name)}
    </a>;

  let definedTypeSelector =
      (name: string, filterFunction: Definition.t => bool) =>
    <div className="type-selector-choices">
      <h3> {ReasonReact.string(name ++ " Types")} </h3>
      {ReasonReact.array(
         Belt.Array.map(
           Belt.Map.String.toArray(
             Belt.Map.String.keep(
               definitions,
               (_definitionID: DefinitionID.t, definition: Definition.t) =>
               filterFunction(definition)
             ),
           ),
           ((definitionID: DefinitionID.t, definition: Definition.t)) =>
           <a
             key=definitionID
             className={
               ValueTypeHasDefinitionID.f(valueType, definitionID)
                 ? "selected" : ""
             }
             onClick={_event => changeType(DefinedValueType(definitionID))}>
             {ReasonReact.string(
                DefinitionGetDisplayName.f(definition, "en"),
              )}
           </a>
         ),
       )}
    </div>;

  <div className="type-selector">
    <a onClick={_event => dispatch(Toggle)}>
      {ReasonReact.string(
         ValueTypeGetDisplayName.f(valueType, definitions, "en"),
       )}
    </a>
    {state.opened
       ? <div className="type-selector-menu">
           <div className="type-selector-categories">
             <h3> {ReasonReact.string("Category")} </h3>
             {renderCategory("Any", AnyCategory)}
             {renderCategory("Text", TextCategory)}
             {renderCategory("Number", NumberCategory)}
             {renderCategory("Record", RecordCategory)}
             {renderCategory("Function", FunctionCategory)}
           </div>
           {switch (state.category) {
            | RecordCategory =>
              definedTypeSelector("Record", DefinitionIsRecordType.f)
            | FunctionCategory =>
              definedTypeSelector("Function", DefinitionIsInterface.f)
            | _ => ReasonReact.null
            }}
         </div>
       : ReasonReact.null}
  </div>;
};
