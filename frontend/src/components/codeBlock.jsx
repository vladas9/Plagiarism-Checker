import { stripIndent } from "common-tags";

export default function CodeBlock(props) {
    console.log(props.code);
    return (
        <div className="mb-8 max-w-screen-md mx-auto text-left">
            <pre className="bg-gray-900 p-4 rounded-lg">
                <div className="flex justify-between items-center mb-2">
                    <span className="text-gray-400">{props.path}</span>
                </div>
                <code className="text-white">
                {stripIndent(props.code)}
                </code>
            </pre>
        </div>
    )
}