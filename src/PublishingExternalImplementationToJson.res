let f = (publishingExternal: PublishingExternalImplementation.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("type", string("interface")),
    ("inputs", PublishingValueTypeListToJson.f(publishingExternal.interface.inputs)),
    ("outputs", PublishingValueTypeListToJson.f(publishingExternal.interface.outputs)),
  })
}
