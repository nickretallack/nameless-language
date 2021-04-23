let f = (string: string): PrimitiveValueType.t =>
  switch string {
  | "integer" => IntegerType
  | "number" => NumberType
  | "text" => TextType
  | _ => raise(Exception.UnknownTypeName("PrimitiveValueTypeFromString", string))
  }
