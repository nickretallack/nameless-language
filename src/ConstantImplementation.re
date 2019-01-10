open Definition;
open AppActions;
open Helpers;

let component = ReasonReact.statelessComponent("ConstantDefinition");

let round = (x: float) => int_of_float(x +. 0.5);

let convertPrimitiveValueType =
    (primitiveValue: primitiveValue, typeName: string): primitiveValue =>
  switch (typeName) {
  | "text" => TextValue(primitiveValueToString(primitiveValue))
  | "number" =>
    NumberValue(
      try (float_of_string(primitiveValueToString(primitiveValue))) {
      | Failure(_) => 0.0
      },
    )
  | "integer" =>
    IntegerValue(
      switch (primitiveValue) {
      | NumberValue(value) => round(value)
      | _ => int_of_string(primitiveValueToString(primitiveValue))
      },
    )
  | _ => raise(Not_found)
  };

let make = (~implementation: primitiveValue, ~emit, _children) => {
  ...component,
  render: _self => {
    let changeType = event =>
      emit(
        ChangeConstantValue(
          convertPrimitiveValueType(implementation, getEventValue(event)),
        ),
      );

    let changeValue = value => emit(ChangeConstantValue(value));

    let typeName = primitiveValueToTypeString(implementation);

    <div>
      <h1> {ReasonReact.string("Constant")} </h1>
      <div> {ReasonReact.string("Type:")} </div>
      <select value=typeName onChange=changeType>
        {ReasonReact.array(
           Belt.Array.map(primitiveValueTypes, primitiveValueType =>
             <option value={primitiveValueTypeToString(primitiveValueType)}>
               {ReasonReact.string(
                  displayPrimitiveValueType(primitiveValueType),
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
           onChange={event => changeValue(TextValue(getEventValue(event)))}
         />
       | NumberValue(value) =>
         <input
           type_="number"
           value={floatToString(value)}
           onChange={event =>
             changeValue(
               NumberValue(
                 try (float_of_string(getEventValue(event))) {
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
                 try (int_of_string(getEventValue(event))) {
                 | Failure(_) => 0
                 },
               ),
             )
           }
         />
       }}
    </div>;
  },
};
