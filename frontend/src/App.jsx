import React from 'react';
import { FileUpload } from 'primereact/fileupload';
import Verdict from "./components/verdict";

export default function App() {
    return (<Verdict />
        // <div className="card">
        //     <FileUpload 
        //         name="demo[]" 
        //         url={'http://192.168.188.76:8000'} 
        //         onSelect={console.log}
        //         onUpload={console.log}
        //         multiple 
        //         accept=".c,.txt"
        //         maxFileSize={1000000} 
        //         emptyTemplate={
        //             <p className="m-0">Drag and drop files to here to upload.</p>
        //         } 
        //     />
        // </div>
    )
}
    