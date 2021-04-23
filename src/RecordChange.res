let f = (record: RecordType.t, nibID: NibID.t, valueType: ValueType.t): RecordType.t =>
  Belt.Map.String.set(record, nibID, valueType)
