@react.component
let make = (~implementation: PrimitiveValue.t, ~emit) => {
  let changeType = event =>
    emit(
      DefinitionAction.ChangeConstantValue(
        ConvertPrimitiveValueType.f(implementation, EventGetValue.f(event)),
      ),
    )

  let changeValue = value => emit(ChangeConstantValue(value))

  let typeName = PrimitiveValueToTypeString.f(implementation)

  <div>
    <h1> {React.string("Constant")} </h1>
    <div> {React.string("Type:")} </div>
    <select value=typeName onChange=changeType>
      {React.array(
        Belt.Array.map(PrimitiveValueType.list, primitiveValueType =>
          <option
            value={PrimitiveValueTypeToString.f(primitiveValueType)}
            key={PrimitiveValueTypeToString.f(primitiveValueType)}>
            {React.string(PrimitiveValueTypeGetDisplayName.f(primitiveValueType))}
          </option>
        ),
      )}
    </select>
    <div> {React.string("Value:")} </div>
    {switch implementation {
    | TextValue(value) =>
      <input
        type_="text" value onChange={event => changeValue(TextValue(EventGetValue.f(event)))}
      />
    | NumberValue(value) =>
      <input
        type_="number"
        value={FloatToString.f(value)}
        onChange={event =>
          changeValue(
            NumberValue(
              try float_of_string(EventGetValue.f(event)) catch {
              | Failure(_) => 0.0
              },
            ),
          )}
      />
    | IntegerValue(value) =>
      <input
        type_="number"
        value={string_of_int(value)}
        onChange={event =>
          changeValue(
            IntegerValue(
              try int_of_string(EventGetValue.f(event)) catch {
              | Failure(_) => 0
              },
            ),
          )}
      />
    }}
  </div>
}
