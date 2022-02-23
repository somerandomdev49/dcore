# Resources

## Server/Client Architecture

- Existing protocol: [Ultima Online Protocol](http://necrotoolz.sourceforge.net/kairpacketguide/)


Outated: 
~~When entering the world, the client requests all of the nearby entities from the
world. UDP for movement and simple tasks that don't require immediate approval
from the server. TCP for spell casting, trading and basically everything else.
There should be a place to view the stock market from the offical website.~~

UDP.

```csharp
class ActorEntityAttachment: EntityAttachmentBase<ActorEntityAttachment>
{
	[OnPostInitialize { ControllerC = this.Entity.GetComponent<>(); }]
	ActorControllerComponent ControllerC;

	[OnPostInitialize { TransformC = this.Entity.GetComponent<>(); }]
	TransformComponent TransformC;

	ActorControllerComponent.InputRequest lastInputRequest;

	[Handler<ActorControllerComponent>("OnPostUpdate")]
	[OnPostInitialize { AttachHandler(OnActorPostUpdate, this.ControllerC); }]
	void OnActorPostUpdate(ActorControllerComponent.InputRequest req)
	{
		lastInputRequest = req;
	}

	[Handler<NetworkManager>("OnPacketSend")]
	[OnPostInitialize { AttachHandler(OnNetworkPacketSend, NetworkManager.Instance.Client); }]
	void OnNetworkPacketSend([Cast<PacketSender>] ActorPacketSender sender)
	{
		if(lastInputRequest.MovementAwaitingValitation)
			sender.SendMovementValidationRequest(TransformC.GetPosition());
		
		if(lastInputRequest.SpellCastAwaitingValidation)
			sender.SendSpellCastValidationRequest(lastInputRequest.SpellCast.Spell.Data);
	}

	[Handler<NetworkManager>("OnPacketReceive")]
	[OnPostInitialize { AttachHandler(OnNetworkPacketReceive, NetworkManager.Instance.Client); }]
	void OnNetworkPacketReceive([Cast<PacketReceiver>] ActorPacketReceiver receiver)
	{
		if(lastInputRequest.SpellCastAwaitingValitation
		&& receiver.SpellCastValidationReceived)
		{
			if(receiver.SpellCastValidation.Approved) lastInputRequest.SpellCast.Approve();
			else lastInputRequest.SpellCast.Reject(
				receiver.SpellCastValidation.GetRejectionReason()
			);
		}
		
		if(lastInputRequest.MovementAwaitingValitation
		&& receiver.MovementValidationReceived)
		{
			if(!receiver.MovementValidation.Approved)
				ControllerC.SetPositionNow(receiver.MovementValidation.GetCorrectLocation());
		}
	}
}
```
