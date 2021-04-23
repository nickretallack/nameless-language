@react.component
let make = (
  ~valueType: ValueType.t,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~changeType: ValueType.t => unit,
) => {
  let (state, dispatch) = ReactUpdate.useReducer(
    {
      open TypeSelectorState
      {
        opened: false,
        category: TypeToCategory.f(valueType, definitions),
      }
    },
    TypeSelectorReducer.f(changeType),
  )

  let renderCategory = (name: string, category: TypeSelectorCategory.t) =>
    <a
      onClick={_event => dispatch(SelectCategory(category))}
      className={state.category == category ? "selected" : ""}>
      {React.string(name)}
    </a>

  let definedTypeSelector = (name: string, filterFunction: Definition.t => bool) =>
    <div className="type-selector-choices">
      <h3> {React.string(name ++ " Types")} </h3>
      {React.array(
        Belt.Array.map(
          Belt.Map.String.toArray(
            Belt.Map.String.keep(definitions, (
              _definitionID: DefinitionID.t,
              definition: Definition.t,
            ) => filterFunction(definition)),
          ),
          ((definitionID: DefinitionID.t, definition: Definition.t)) =>
            <a
              key=definitionID
              className={ValueTypeHasDefinitionID.f(valueType, definitionID) ? "selected" : ""}
              onClick={_event => changeType(DefinedValueType(definitionID))}>
              {React.string(DefinitionGetDisplayName.f(definition, languageName))}
            </a>,
        ),
      )}
    </div>

  <div className="type-selector">
    <a onClick={_event => dispatch(Toggle)}>
      {React.string(ValueTypeGetDisplayName.f(valueType, definitions, languageName))}
    </a>
    {state.opened
      ? <div className="type-selector-menu">
          <div className="type-selector-categories">
            <h3> {React.string("Category")} </h3>
            {renderCategory("Any", AnyCategory)}
            {renderCategory("Text", TextCategory)}
            {renderCategory("Number", NumberCategory)}
            {renderCategory("Record", RecordCategory)}
            {renderCategory("Function", FunctionCategory)}
            {renderCategory("Label", LabelCategory)}
            {renderCategory("Union", UnionCategory)}
          </div>
          {switch state.category {
          | RecordCategory => definedTypeSelector("Record", DefinitionIsRecordType.f)
          | FunctionCategory => definedTypeSelector("Function", DefinitionIsInterface.f)
          | LabelCategory => definedTypeSelector("Label", DefinitionIsLabel.f)
          | UnionCategory => definedTypeSelector("Union", DefinitionIsUnion.f)
          | _ => React.null
          }}
        </div>
      : React.null}
  </div>
}
