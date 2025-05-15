"use client";
import { trackFallbackParamAccessed } from "next/dist/server/app-render/dynamic-rendering";
import Image from "next/image";
import { useState, useEffect } from "react";


async function sendTextToLCD(text) {

	const arduinoIP = "http://172.20.10.3";

	if (text === "") {
		alert("Please enter some text!");
		return;
	}

	try {
		fetch(`${arduinoIP}/?text=${encodeURIComponent(text)}`)
			.then(response => response.text())
			.then(data => {
				console.log("Response from Arduino:", data);
			})
			.catch(error => {
				console.error("Error:", error);
			});
	}
	catch (err) {
		console.error("Failed to send text:", err);
	}
}

export default function Home() {

	const [text, setText] = useState("gyundyuz");

	useEffect(() => {
	}, [text]);

	return (
		<>
			<main className="flex justify-center flex-col items-center my-[20%]">

				<h1 className="text-4xl mb-5">Please insert some text</h1>
				<div className="
						flex flex-col justify-center items-center">

					<input type="text" value={text} onChange={(e) => setText(e.target.value)} className="border-2 border-gray-300 rounded px-4 py-2 mb-5" />
					<input type="button" onClick={() => { sendTextToLCD(text); }} value="Send" className="bg-blue-500 text-white px-4 py-2 rounded" />
				</div>

			</main>

		</>
	);
}
