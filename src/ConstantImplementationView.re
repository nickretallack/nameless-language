[@react.component]
let make = (~implementation: PrimitiveValue.t, ~emit) => {
  let changeType = event =>
    emit(
      DefinitionAction.ChangeConstantValue(
        ConvertPrimitiveValueType.f(implementation, EventGetValue.f(event)),
      ),
    );

  let changeValue = value => emit(ChangeConstantValue(value));

  let typeName = PrimitiveValueToTypeString.f(implementation);

  <div>
    <h1> {ReasonReact.string("Constant")} </h1>
    <div> {ReasonReact.string("Type:")} </div>
    <select value=typeName onChange=changeType>
      {ReasonReact.array(
         Belt.Array.map(PrimitiveValueType.list, primitiveValueType =>
           <option
             value={PrimitiveValueTypeToString.f(primitiveValueType)}
             key={PrimitiveValueTypeToString.f(primitiveValueType)}>
             {ReasonReact.string(
                PrimitiveValueTypeGetDisplayName.f(primitiveValueType),
              )}
           </option>
         ),
       )}
    </select>
    <div> {ReasonReact.string("Value:")} </div>
    {switch (implementation) {
     | TextValue(value) =>
       <input
         type_="text"
         value
         onChange={event => changeValue(TextValue(EventGetValue.f(event)))}
       />
     | NumberValue(value) =>
       <input
         type_="number"
         value={FloatToString.f(value)}
         onChange={event =>
           changeValue(
             NumberValue(
               try (float_of_string(EventGetValue.f(event))) {
               | Failure(_) => 0.0
               },
             ),
           )
         }
       />
     | IntegerValue(value) =>
       <input
         type_="number"
         value={string_of_int(value)}
         onChange={event =>
           changeValue(
             IntegerValue(
               try (int_of_string(EventGetValue.f(event))) {
               | Failure(_) => 0
               },
             ),
           )
         }
       />
     }}
  </div>;
};
