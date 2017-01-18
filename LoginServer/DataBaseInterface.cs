using MySql.Data.MySqlClient;

namespace LoginServer
{
    public partial class DataBaseInterface
    {
        public readonly MySqlConnection Connection;

        public DataBaseInterface(string host, uint port, string database, string login, string password)
        {
            string connectString = string.Format("Server={0};Port={1};database={2};UID={3};password={4};Allow User Variables=True",
                host, port, database, login, password);

            Connection = new MySqlConnection(connectString);
            Connection.Open();
        }

        public bool Execute(string query, params object[] args)
        {
            using (MySqlCommand cmd = new MySqlCommand(string.Format(query, args), Connection))
                return cmd.ExecuteNonQuery() != 0;
        }

        public T Get<T>(string query, params object[] args)
        {
            return Get<T>(query, out bool temp, args);
        }

        public T Get<T>(string query, out bool success, params object[] args)
        {
            T result = default(T);
            success = false;

            using (var cmd = new MySqlCommand(string.Format(query, args), Connection))
            using (var reader = cmd.ExecuteReader())
                if (reader != null && reader.HasRows)
                {
                    reader.Read();
                    result = ReadValue<T>(reader, 0);
                    success = true;
                }

            return result;
        }

        public T ReadValue<T>(System.Data.Common.DbDataReader reader, int index)
        {
            object obj = reader.GetValue(index);
            if (obj is T)
                return (T)obj;

            try
            {
                return (T)obj;
            }
            catch { }

            return default(T);
        }
    }
}
