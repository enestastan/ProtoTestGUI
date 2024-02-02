const net = require('net');
const fs = require('fs');
const messages = require('./Protocol/Generated/descriptor_pb');

const socketPath = '/tmp/mysocket'; // Replace with the server socket path

// Create a Uint8Array buffer to accumulate received data
let receivedDataBuffer = new Uint8Array();

// Check if the socket file exists
if (!fs.existsSync(socketPath)) {
	console.error(`Server socket does not exist at ${socketPath}`);
	process.exit(1);
}

// Create a Unix domain socket client
const client = net.createConnection({ path: socketPath });

// Handle data received from the server
client.on('data', (data) => {
	receivedDataBuffer = new Uint8Array([...receivedDataBuffer, ...data]);
});

// Handle connection termination
client.on('end', () => {
	var message = messages.FileDescriptorCustom.deserializeBinary(receivedDataBuffer);
	printProtoFile(message);
});

// Handle errors
client.on('error', (err) => {
	console.error(`Socket error: ${err.message}`);
});

function printProtoFile(descriptor) {
	console.log("syntax = " + descriptor.getSyntax());
	console.log("package = " + descriptor.getPackage());
	console.log("");

	descriptor.getEnumTypeList().forEach(enumm => {
		console.log(enumm.getName() + " {");
		enumm.getValueList().forEach(value => {
			console.log("\t" + value.getName() + " = " + value.getNumber() + ";")
		})
		console.log("}");
		console.log("");
	});

	descriptor.getMessageTypeList().forEach(msg => {
		console.log(msg.getName() + " {");
		msg.getFieldList().forEach(field => {
			console.log("\t" + field.getTypeName() + " " + field.getName() + " = " + field.getNumber() + ";");
		});
		console.log("}");
		console.log("");
	});

}