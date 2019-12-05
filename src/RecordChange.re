let f = (record: Record.t, nibID: NibID.t, valueType: ValueType.t): Record.t =>
  Belt.Map.String.set(record, nibID, valueType);
