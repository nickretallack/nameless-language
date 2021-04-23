let f = (interface: PublishingInterface.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("type", string("interface")),
    ("inputs", PublishingValueTypeListToJson.f(interface.inputs)),
    ("outputs", PublishingValueTypeListToJson.f(interface.outputs)),
  })
}
