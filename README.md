
<h1>MP3 Tag Reader</h1>
<p>A C program to <span class="highlight">view</span> and <span class="highlight">edit</span> ID3v2.3 metadata of MP3 files.  
It allows users to read and update tags like <span class="highlight">Title</span>, <span class="highlight">Artist</span>, <span class="highlight">Album</span>, <span class="highlight">Year</span>, and <span class="highlight">Genre</span> via a CLI interface.</p>

<h2>Features</h2>
<ul>
    <li>View MP3 metadata (ID3v2.3 only).</li>
    <li>Edit MP3 tags safely using temporary file handling.</li>
    <li>Command-line interface with validation and error messages.</li>
    <li>Help menu for usage guidance.</li>
    <li>Supports common tags: <span class="badge">Title</span> <span class="badge">Artist</span> <span class="badge">Album</span> <span class="badge">Year</span> <span class="badge">Genre</span>.</li>
</ul>

<h2>Usage</h2>

<h3>View MP3 Metadata</h3>
<pre><code>./a.out -v &lt;filename&gt;.mp3</code></pre>

<h3>Edit MP3 Metadata</h3>
<pre><code>./a.out -e &lt;tag&gt; &lt;new_value&gt; &lt;filename&gt;.mp3</code></pre>

<p>Supported tags:</p>
<table>
<thead>
<tr><th>Option</th><th>Tag</th></tr>
</thead>
<tbody>
<tr><td>-t</td><td>Title</td></tr>
<tr><td>-a</td><td>Artist</td></tr>
<tr><td>-A</td><td>Album</td></tr>
<tr><td>-y</td><td>Year</td></tr>
<tr><td>-c</td><td>Genre</td></tr>
</tbody>
</table>

<h3>Help Menu</h3>
<pre><code>./a.out --help</code></pre>

<h2>Project Files</h2>
<ul>
    <li><code>main.c</code> — Main driver with operation selection.</li>
    <li><code>mp3.h</code> — Header file with structures, enums, and function prototypes.</li>
    <li><code>view.c</code> — Functions to read and display MP3 tags.</li>
    <li><code>edit.c</code> — Functions to safely edit MP3 tags.</li>
    <li><code>check_operation.c</code> — Command-line argument validation and error handling.</li>
</ul>

<h2>Requirements</h2>
<ul>
    <li>Linux or Unix-like environment.</li>
    <li>C compiler (gcc recommended).</li>
    <li>MP3 files with ID3v2.3 tags.</li>
</ul>

<h2>Compilation</h2>
<pre><code>gcc main.c view.c edit.c check_operation.c -o mp3tagreader
./mp3tagreader [options]</code></pre>


</body>
</html>
