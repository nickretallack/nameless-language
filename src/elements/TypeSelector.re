[%%debugger.chrome];
open Definition;

type category =
  | NumberCategory
  | TextCategory
  | RecordCategory
  | FunctionCategory
  | AnyCategory;

type state = {
  opened: bool,
  category,
};

type action =
  | Toggle
  | SelectCategory(category);

let categoryFromType =
    (valueType: valueType, definitions: definitions): category =>
  switch (valueType) {
  | PrimitiveValueType(primitiveValueType) =>
    switch (primitiveValueType) {
    | NumberType => NumberCategory
    | IntegerType => NumberCategory
    | TextType => TextCategory
    }
  | DefinedValueType(definitionID) =>
    let definition = Belt.Map.String.getExn(definitions, definitionID);
    switch (definition.implementation) {
    | InterfaceImplementation(_) => FunctionCategory
    | RecordTypeImplementation(_) => RecordCategory
    | _ => raise(Not_found)
    };
  | AnyType => AnyCategory
  };

let hasDefinitionID = (valueType: valueType, definitionID: definitionID) =>
  switch (valueType) {
  | PrimitiveValueType(_)
  | AnyType => false
  | DefinedValueType(theDefinitionID) => definitionID == theDefinitionID
  };

let component = ReasonReact.reducerComponent("TypeSelector");

let isRecordType = (definition: definition): bool =>
  switch (definition.implementation) {
  | RecordTypeImplementation(_) => true
  | _ => false
  };

let isInterface = (definition: definition): bool =>
  switch (definition.implementation) {
  | InterfaceImplementation(_) => true
  | _ => false
  };

let make =
    (~valueType: valueType, ~definitions: definitions, ~changeType, _children) => {
  ...component,
  initialState: () => {
    opened: false,
    category: categoryFromType(valueType, definitions),
  },
  reducer: (action: action, state: state) =>
    switch (action) {
    | Toggle => ReasonReact.Update({...state, opened: !state.opened})
    | SelectCategory(category) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, category},
        _ =>
          switch (category) {
          | TextCategory => changeType(PrimitiveValueType(TextType))
          | NumberCategory => changeType(PrimitiveValueType(NumberType))
          | _ => ()
          },
      )
    },
  render: self => {
    let renderCategory = (name: string, category: category) =>
      <a
        onClick={_event => self.send(SelectCategory(category))}
        className={self.state.category == category ? "selected" : ""}>
        {ReasonReact.string(name)}
      </a>;

    let definedTypeSelector =
        (name: string, filterFunction: definition => bool) =>
      <div className="type-selector-choices">
        <h3> {ReasonReact.string(name ++ " Types")} </h3>
        {ReasonReact.array(
           Belt.Array.map(
             Belt.Map.String.toArray(
               Belt.Map.String.keep(
                 definitions,
                 (_definitionID: definitionID, definition: definition) =>
                 filterFunction(definition)
               ),
             ),
             ((definitionID: definitionID, definition: definition)) =>
             <a
               key=definitionID
               className={
                 hasDefinitionID(valueType, definitionID) ? "selected" : ""
               }
               onClick={_event => changeType(DefinedValueType(definitionID))}>
               {ReasonReact.string(getDisplayName(definition, "en"))}
             </a>
           ),
         )}
      </div>;

    <div className="type-selector">
      <a onClick={_event => self.send(Toggle)}>
        {ReasonReact.string(displayValueType(valueType, definitions, "en"))}
      </a>
      {self.state.opened ?
         <div className="type-selector-menu">
           <div className="type-selector-categories">
             <h3> {ReasonReact.string("Category")} </h3>
             {renderCategory("Any", AnyCategory)}
             {renderCategory("Text", TextCategory)}
             {renderCategory("Number", NumberCategory)}
             {renderCategory("Record", RecordCategory)}
             {renderCategory("Function", FunctionCategory)}
           </div>
           {switch (self.state.category) {
            | RecordCategory => definedTypeSelector("Record", isRecordType)
            | FunctionCategory => definedTypeSelector("Function", isInterface)
            | _ => ReasonReact.null
            }}
         </div> :
         ReasonReact.null}
    </div>;
  },
};
