@react.component
let make = (
  ~definitions: DefinitionMap.t,
  ~typeSet: TypeSet.t,
  ~languageName: LanguageName.t,
  ~emit,
) => {
  let (selectedType, setSelectedType) = React.useState(() => ValueType.AnyType)

  <div>
    <h1> {React.string("Union")} </h1>
    <TypeSelectorView
      valueType=selectedType
      definitions
      languageName
      changeType={valueType => setSelectedType(_ => valueType)}
    />
    <button onClick={_event => emit(DefinitionAction.AddUnionType(selectedType))}>
      {React.string("Add Type")}
    </button>
    <table>
      <tbody>
        {React.array(
          Belt.Array.map(Belt.Set.toArray(typeSet), valueType =>
            <tr key={Json.stringify(ValueTypeToJson.f(valueType))}>
              <td>
                <a onClick={_event => emit(DefinitionAction.RemoveUnionType(valueType))}>
                  {React.string("x")}
                </a>
              </td>
              <td>
                {React.string(ValueTypeGetDisplayName.f(valueType, definitions, languageName))}
              </td>
            </tr>
          ),
        )}
      </tbody>
    </table>
  </div>
}
