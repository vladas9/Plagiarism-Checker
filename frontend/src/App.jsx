import React from 'react';
import { FileUpload } from 'primereact/fileupload';

export default function UnstyledDemo() {
    return (
        <div className="card">
            <FileUpload 
                name="demo[]" 
                url={'/api/upload'} 
                onSelect={console.log}
                multiple 
                accept=".c,.txt"
                maxFileSize={1000000} 
                emptyTemplate={
                    <p className="m-0">Drag and drop files to here to upload.</p>
                } 
            />
        </div>
    )
}
    