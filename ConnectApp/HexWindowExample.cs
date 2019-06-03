using DoorController;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConnectApp
{
    public class HexWindowExample
    {
        public HexWindowExample() { }

        public ObservableCollection<ByteRow> Rows { get; } = new ObservableCollection<ByteRow>()
        {
            new ByteRow(new byte[]{(byte)'a',(byte)'b',(byte)'c',(byte)'d',(byte)'e',(byte)'f',(byte)'g',(byte)'h',(byte)'i',(byte)'j',(byte)'k',(byte)'l',(byte)'m',(byte)'n',(byte)'o',(byte)'p'}),
            new ByteRow(new byte[]{(byte)'a',(byte)'b',(byte)'c',(byte)'d',(byte)'e',(byte)'f',(byte)'g',(byte)'h',(byte)'i',(byte)'j',(byte)'k',(byte)'l',(byte)'m',(byte)'n',(byte)'o',(byte)'p'}),
            new ByteRow(new byte[]{(byte)'a',(byte)'b',(byte)'c',(byte)'d',(byte)'e',(byte)'f',(byte)'g',(byte)'h',(byte)'i',(byte)'j',(byte)'k',(byte)'l',(byte)'m',(byte)'n',(byte)'o',(byte)'p'})
        };
    }
}
