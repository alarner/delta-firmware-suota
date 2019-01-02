param([String]$com="COM5")
echo "starting..."
$port= new-Object System.IO.Ports.SerialPort $com,115200,None,8,one
$port.Open()
while($true) {
    $port.ReadLine()
}
echo "done"
